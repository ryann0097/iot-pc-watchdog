class User:
    """
    Classe que representa um usuário com ID, código e nome.
    """

    def __init__(self, user_id: int, name: str, url: str):
        self.user_id = user_id
        self.name = name
        self.url = url

    def __str__(self):
        return f"User(id={self.user_id}, name='{self.name}', url='{self.url}')"

    def to_dict(self):
        """
        Converte o objeto User em um dicionário.
        :return: Dicionário com os dados do usuário.
        """
        return {
            "id": self.user_id,
            "url": self.url,
            "name": self.name
        }

    def update_name(self, new_name: str):
        """
        Atualiza o nome do usuário.

        :param new_name: Novo nome do usuário.
        """
        if not new_name:
            raise ValueError("O nome não pode ser vazio.")
        self.name = new_name
